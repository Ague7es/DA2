//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#include "../include/WebBuilder.h"

#include <algorithm>

std::vector<Web> WebBuilder::build(const std::vector<LiveRange>& liveRanges) {
    std::vector<Web> webs;
    int nextWebId = 0;

    for (const auto& range : liveRanges) {

        bool merged = false;

        for (auto& web : webs) {

            if (web.variable != range.variable) {
                continue;
            }

            if (overlaps(web, range)) {
                merge(web, range);
                merged = true;
                break;
            }
        }

        if (!merged) {
            Web web;

            web.id = nextWebId++;
            web.variable = range.variable;
            web.points = range.points;

            webs.push_back(web);
        }
    }

    return webs;
}

bool WebBuilder::containsLine(const Web& web, int line) const {
    for (const auto& point : web.points) {
        if (point.line == line) {
            return true;
        }
    }

    return false;
}

bool WebBuilder::overlaps(const Web& web,
                          const LiveRange& range) const {

    for (const auto& point : range.points) {

        if (containsLine(web, point.line)) {
            return true;
        }
    }

    return false;
}

void WebBuilder::merge(Web& web,
                       const LiveRange& range) const {

    for (const auto& point : range.points) {

        bool exists = false;

        for (const auto& existing : web.points) {

            if (existing.line == point.line) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            web.points.push_back(point);
        }
    }

    std::sort(web.points.begin(),
              web.points.end(),
              [](const ProgramPoint& a, const ProgramPoint& b) {
                  return a.line < b.line;
              });
}