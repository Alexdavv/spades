#pragma once

#include "io/single_read.hpp"

namespace debruijn_graph {

typedef std::string bin_id;
typedef std::string contig_id;
typedef std::vector<bin_id> Bins;
typedef std::pair<contig_id, Bins> ContigAnnotation;

inline contig_id GetId(const io::SingleRead& contig) {
     std::string name = contig.name();
     size_t pos = name.find("_ID_");
     VERIFY(pos != std::string::npos);
     size_t start = pos + 4;
     VERIFY(start < name.size());
     return name.substr(start, name.size() - start);
}

inline contig_id GetBaseId(const contig_id& id) {
    size_t pos = id.find('_');
    return id.substr(0, pos);
}

}
