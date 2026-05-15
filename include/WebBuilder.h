//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_WEBBUILDER_H
#define DA2_WEBBUILDER_H

#include <vector>
#include "LiveRange.h"
#include "Web.h"

class WebBuilder {
public:
    std::vector<Web> build(const std::vector<LiveRange>& liveRanges);

private:
    bool containsLine(const Web& web, int line) const;
    bool overlaps(const Web& web, const LiveRange& range) const;
    void merge(Web& web, const LiveRange& range) const;
};

#endif //DA2_WEBBUILDER_H