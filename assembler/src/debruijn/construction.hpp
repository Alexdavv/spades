//***************************************************************************
//* Copyright (c) 2011-2013 Saint-Petersburg Academic University
//* All Rights Reserved
//* See file LICENSE for details.
//****************************************************************************

/*
 * construction.hpp
 *
 *  Created on: 1 Sep 2011
 *      Author: valery
 */

#pragma once

#include "standard.hpp"
#include "io/easy_reader.hpp"
#include "io/vector_reader.hpp"
#include "omni_labelers.hpp"
#include "dataset_readers.hpp"
#include "config_common.hpp" // FIXME: Get rid of this!

namespace debruijn_graph {

void exec_construction(PairedReadStream& stream, conj_graph_pack& gp,
    total_labeler& tl, PairedIndexT& paired_index);

} // namespace debruijn_graph

// todo: move impl to *.cpp

namespace debruijn_graph {

template<class Read>
void construct_graph(io::ReadStreamVector< io::IReader<Read> >& streams,
                     conj_graph_pack& gp, ReadStream* contigs_stream = 0) {
  INFO("STAGE == Constructing Graph");
  size_t rl = ConstructGraphWithCoverage<Read>(cfg::get().K, streams, gp.g,
                                               gp.index, contigs_stream);
  if (!cfg::get().ds.RL()) {
    INFO("Figured out: read length = " << rl);
    cfg::get_writable().ds.set_RL(rl);
  } else if (cfg::get().ds.RL() != rl)
    WARN("In datasets.info, wrong RL is specified: " << cfg::get().ds.RL() << ", not " << rl);
}

std::string estimated_param_filename(const string& prefix) {
  return prefix + "_est_params.info";
}

void load_estimated_params(const string& prefix) {
  std::string filename = estimated_param_filename(prefix);
  //todo think of better architecture
  if (FileExists(filename)) {
    boost::optional<size_t> val1; boost::optional<double> val2;
    load_param(filename, "RL", val1);
    if (val1)
      cfg::get_writable().ds.set_RL(*val1);
    load_param(filename, "IS", val1);
    if (val1)
      cfg::get_writable().ds.set_IS(*val1);
    load_param(filename, "is_var", val2);
    if (val2)
      cfg::get_writable().ds.set_is_var(*val2);
    load_param(filename, "avg_coverage", val2);
    if (val2)
      cfg::get_writable().ds.set_avg_coverage(*val2);
    load_param(filename, "median", val2);
    if (val2)
      cfg::get_writable().ds.set_median(*val2);
    load_param(filename, "mad", val2);
    if (val2)
      cfg::get_writable().ds.set_mad(*val2);
    std::map<int, size_t> val3;
    load_param_map(filename, "hist", val3);
    cfg::get_writable().ds.set_hist(val3);
  }
}

void write_estimated_params(const string& prefix) {
  std::string filename = estimated_param_filename(prefix);
  write_param(filename, "RL", cfg::get().ds.RL());
  write_param(filename, "IS", cfg::get().ds.IS());
  write_param(filename, "is_var", cfg::get().ds.is_var());
  write_param(filename, "avg_coverage", cfg::get().ds.avg_coverage());
  write_param(filename, "median", cfg::get().ds.median());
  write_param(filename, "mad", cfg::get().ds.mad());
  write_param_map(filename, "hist", cfg::get().ds.hist());
}

void return_estimated_params() {
	write_estimated_params(cfg::get().output_dir + "/");
}


void load_lib_data(const string& prefix) {
  std::string filename = estimated_param_filename(prefix);

  if (!FileExists(filename)) {
      WARN("Estimates params config " << prefix << " does not exist");
  }

  boost::optional<size_t> lib_count;
  load_param(filename, "lib_count", lib_count);
  if (!lib_count || lib_count != cfg::get().ds.reads.lib_count()) {
      WARN("Estimated params file seems to be incorrect");
      return;
  }

  for (size_t i = 0; i < cfg::get().ds.reads.lib_count(); ++i) {
      boost::optional<size_t> sizet_val;
      boost::optional<double> double_val;

      load_param(filename, "read_length_" + ToString(i), sizet_val);
      if (sizet_val) {
          cfg::get_writable().ds.reads[i].data().read_length = *sizet_val;
      }
      load_param(filename, "insert_size_" + ToString(i), double_val);
      if (double_val) {
          cfg::get_writable().ds.reads[i].data().mean_insert_size = *double_val;
      }
      load_param(filename, "insert_size_deviation_" + ToString(i), double_val);
      if (double_val) {
          cfg::get_writable().ds.reads[i].data().insert_size_deviation = *double_val;
      }
      load_param(filename, "insert_size_median_" + ToString(i), double_val);
      if (double_val) {
          cfg::get_writable().ds.reads[i].data().median_insert_size = *double_val;
      }
      load_param(filename, "insert_size_mad_" + ToString(i), double_val);
      if (double_val) {
          cfg::get_writable().ds.reads[i].data().insert_size_mad = *double_val;
      }
      load_param(filename, "average_coverage_" + ToString(i), double_val);
      if (double_val) {
          cfg::get_writable().ds.reads[i].data().average_coverage = *double_val;
      }

      load_param_map(filename, "histogram_" + ToString(i), cfg::get_writable().ds.reads[i].data().insert_size_distribution);
  }

}

void write_lib_data(const string& prefix) {
  std::string filename = estimated_param_filename(prefix);

  write_param(filename, "lib_count", cfg::get().ds.reads.lib_count());

  for (size_t i = 0; i < cfg::get().ds.reads.lib_count(); ++i) {
      write_param(filename, "read_length_" + ToString(i), cfg::get().ds.reads[i].data().read_length);
      write_param(filename, "insert_size_" + ToString(i), cfg::get().ds.reads[i].data().mean_insert_size);
      write_param(filename, "insert_size_deviation_" + ToString(i), cfg::get().ds.reads[i].data().insert_size_deviation);
      write_param(filename, "insert_size_median_" + ToString(i), cfg::get().ds.reads[i].data().median_insert_size);
      write_param(filename, "insert_size_mad_" + ToString(i), cfg::get().ds.reads[i].data().insert_size_mad);
      write_param(filename, "average_coverage_" + ToString(i), cfg::get().ds.reads[i].data().average_coverage);
      write_param_map(filename, "histogram_" + ToString(i), cfg::get().ds.reads[i].data().insert_size_distribution);
  }
}


void load_construction(conj_graph_pack& gp, path::files_t* files) {
  string p = path::append_path(cfg::get().load_from, "constructed_graph");
  files->push_back(p);
  ScanGraphPack(p, gp);
  load_lib_data(p);
}

void save_construction(conj_graph_pack& gp) {
  if (cfg::get().make_saves) {
    string p = path::append_path(cfg::get().output_saves, "constructed_graph");
    INFO("Saving current state to " << p);
    PrintGraphPack(p, gp);
    write_lib_data(p);
  }
  return_estimated_params();
}

//boost::optional<string> single_reads_filename(
//    const boost::optional<string>& raw_name, const string& dir) {
//  if (raw_name) {
//    string full_name = dir + *raw_name;
//    if (fileExists(full_name)) {
//      return boost::optional<string>(full_name);
//    }
//  }
//  return boost::none;
//}

void exec_construction(conj_graph_pack& gp) {
  if (cfg::get().entry_point <= ws_construction) {

//    if (cfg::get().etalon_graph_mode) {
//      typedef io::VectorReader<io::SingleRead> GenomeStream;
//      GenomeStream genome_stream(io::SingleRead("genome", gp.genome.str()));
//      std::vector <ReadStream*> streams(1, &genome_stream);
//      construct_graph(streams, gp);
//    } else

    //has to be separate stream for not counting it in coverage
    ReadStream* additional_contigs_stream = 0;
    if (cfg::get().use_additional_contigs) {
      INFO("Contigs from previous K will be used");
      additional_contigs_stream = new io::EasyReader(
          cfg::get().additional_contigs, true);
    }

    std::vector<size_t> libs_for_construction;
    for (size_t i = 0; i < cfg::get().ds.reads.lib_count(); ++i) {
        if (cfg::get().ds.reads[i].type() == io::LibraryType::PairedEnd || cfg::get().ds.reads[i].type() == io::LibraryType::SingleReads) {
            libs_for_construction.push_back(i);
        }
    }

    if (cfg::get().use_multithreading) {
      auto streams = single_binary_readers_for_libs(libs_for_construction, true, true);
      construct_graph<io::SingleReadSeq>(streams, gp,
          additional_contigs_stream);

    } else {
      auto single_stream = single_easy_reader_for_libs(libs_for_construction, true, true);
      io::ReadStreamVector<ReadStream> streams(single_stream.get());
      construct_graph<io::SingleRead>(streams, gp,
          additional_contigs_stream);
    }

    save_construction(gp);
  } else {
    INFO("Loading Construction");

        path::files_t used_files;
    load_construction(gp, &used_files);
    link_files_by_prefix(used_files, cfg::get().output_saves);
//    OnlineVisualizer online(gp);
//    online.run();
  }

  if (cfg::get().developer_mode) {
    if (gp.genome.size() > 0) {
      FillPos(gp, gp.genome, "ref0");
      FillPos(gp, !gp.genome, "ref1");
    }

    if (!cfg::get().pos.contigs_for_threading.empty()
        && FileExists(cfg::get().pos.contigs_for_threading)) {
      FillPosWithRC(gp, cfg::get().pos.contigs_for_threading, "thr_");
    }

    if (!cfg::get().pos.contigs_to_analyze.empty()
        && FileExists(cfg::get().pos.contigs_to_analyze)) {
      FillPosWithRC(gp, cfg::get().pos.contigs_to_analyze, "anlz_");
    }
  }

}

} //namespace debruijn_graph
