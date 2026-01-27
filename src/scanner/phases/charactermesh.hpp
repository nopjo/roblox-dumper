#pragma once

#include "control/control.hpp"
#include "memory/memory.h"
#include "scanner.hpp"
#include "utils/offset_registry.hpp"

#include <chrono>
#include <cstdint>
#include <thread>
#include <unordered_map>
#include <string>

struct LimbMeshInfo
{
    std::uintptr_t instance;
    std::int64_t   expected_id;
    uint8_t expected_bodypart;
};

namespace scanner::phases
{
    inline bool characterMesh(Context& ctx)
    {
        if (!ctx.workspace.is_valid())
        {
            LOG_ERR("Workspace not valid");
            return false;
        }

        auto marco = ctx.workspace.find_first_child("marco");
        if (!marco.is_valid())
        {
            LOG_ERR("Failed to find 'marco' in Workspace");
            return false;
        }

        const std::unordered_map<std::string, LimbMeshInfo> limbs = {
            { "LeftArm",  { marco.find_first_child("LeftArm_CharacterMesh").address,  279174949LL, 2 } },
            { "RightArm", { marco.find_first_child("RightArm_CharacterMesh").address, 279174886LL, 3 } },
            { "Torso",    { marco.find_first_child("Torso_CharacterMesh").address,    376169183LL, 1 } },
            { "LeftLeg",  { marco.find_first_child("LeftLeg_CharacterMesh").address,  319336109LL, 4 } },
            { "RightLeg", { marco.find_first_child("RightLeg_CharacterMesh").address, 319336155LL, 5 } },
        };

        std::unordered_map<std::string, std::uintptr_t> limb_hits;

        for (const auto &[limb_name, info] : limbs)
        {
            if (!info.instance)
            {
                LOG_ERR("Invalid instance for {}", limb_name);
                continue;
            }

            int matches = 0;

            for (std::uintptr_t off = 0xA0; off <= 0x180; off += 0x8)
            {
                std::int64_t value = memory->read<std::int64_t>(info.instance + off);
                if (value == info.expected_id)
                {
                    limb_hits[limb_name] = off;
                    matches++;
                    LOG_INFO("{} match at +0x{:X}", limb_name, off);
                }
            }

            if (matches == 0)
                LOG_WARN("No match for {}", limb_name);
        }

        if (limb_hits.empty())
        {
            LOG_ERR("No CharacterMesh offsets found");
            return false;
        }

        std::unordered_map<std::uintptr_t, int> offset_frequency;

        for (const auto &[_, off] : limb_hits)
            offset_frequency[off]++;

        std::uintptr_t best_offset = 0;
        int max_hits = 0;

        for (const auto &[off, count] : offset_frequency)
        {
            if (count > max_hits)
            {
                max_hits = count;
                best_offset = off;
            }
        }

        offset_registry.add("CharacterMesh", "MeshId", best_offset);

        std::unordered_map<std::string, std::uintptr_t> bodypart_hits;

        for (const auto &[limb_name, info] : limbs)
        {
            if (!info.instance)
            {
                LOG_ERR("Invalid instance for {}", limb_name);
                continue;
            }

            int matches = 0;

            for (std::uintptr_t off = 0xc8; off <= 0xfa; off += 0x1)
            {
                uint8_t value = memory->read<uint8_t>(info.instance + off);

                if (value == info.expected_bodypart)
                {
                    bodypart_hits[limb_name] = off;
                    matches++;
                    LOG_INFO("{} bodypart match at +0x{:X}", limb_name, off);
                }
            }

            if (matches == 0)
                LOG_WARN("No bodypart match for {}", limb_name);
        }

        if (bodypart_hits.empty())
        {
            LOG_ERR("No CharacterMesh BodyPart offsets found");
            return false;
        }

        std::unordered_map<std::uintptr_t, int> bodyPartoffset_frequency;

        for (const auto &[_, off] : bodypart_hits)
            bodyPartoffset_frequency[off]++;

        std::uintptr_t bodyPartbest_offset = 0;
        int bodyPartmax_hits = 0;

        for (const auto &[off, count] : bodyPartoffset_frequency)
        {
            if (count > bodyPartmax_hits)
            {
                bodyPartmax_hits = count;
                bodyPartbest_offset = off;
            }
        }

        offset_registry.add("CharacterMesh", "BodyPart", bodyPartbest_offset);

        return true;
    }
}
