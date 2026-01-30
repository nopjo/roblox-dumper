#include "data_model.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include "process/process.h"
#include "process/rtti/rtti.h"
#include "spdlog/spdlog.h"

namespace dumper::stages::data_model {

    static auto dump_real_data_model() -> bool {
        const auto fake_data_model = process::Memory::read<uintptr_t>(
            process::g_process.get_module_base() +
            *dumper::g_dumper.get_offset("FakeDataModel", "Pointer"));

        if (!fake_data_model) {
            spdlog::error("Failed to read FakeDataModel pointer.");
            return false;
        }

        const auto real_data_model = process::Rtti::find(*fake_data_model, "DataModel@RBX");

        if (!real_data_model) {
            spdlog::error("Failed to get real DataModel.");
            return false;
        }

        dumper::g_dumper.add_offset("FakeDataModel", "RealDataModel", *real_data_model);

        const auto data_model =
            process::Memory::read<uintptr_t>(*fake_data_model + *real_data_model);

        if (!data_model) {
            spdlog::error("Failed to get real DataModel.");
            return false;
        }

        dumper::g_data_model_addr = *data_model;

        return true;
    }

    auto dump() -> bool {
        if (!dump_real_data_model()) {
            return false;
        }

        const auto data = control::client::g_client.get_data_model_information();

        if (!data) {
            spdlog::error("Failed to receive DataModel information via control server.");
            return false;
        }

        FIND_AND_ADD_OFFSET(dumper::g_data_model_addr, DataModel, int64_t, PlaceId, data->place_id,
                            0x1000, 0x8);
        FIND_AND_ADD_OFFSET(dumper::g_data_model_addr, DataModel, int64_t, GameId, data->game_id,
                            0x1000, 0x8);
        FIND_AND_ADD_OFFSET(dumper::g_data_model_addr, DataModel, int64_t, CreatorId,
                            data->creator_id, 0x1000, 0x8);

        const auto job_id = process::helpers::find_string_offset(dumper::g_data_model_addr,
                                                                 data->job_id, 0x1000, 8);

        if (!job_id) {
            spdlog::error("Failed to get JobId from DataModel");
            return false;
        }

        dumper::g_dumper.add_offset("DataModel", "JobId", *job_id);

        FIND_AND_ADD_OFFSET(dumper::g_data_model_addr, DataModel, uint32_t, GameLoaded, 31, 0x1000,
                            0x4);

        const auto ip_address = process::helpers::find_string_by_regex(
            dumper::g_data_model_addr, R"(\d+\.\d+\.\d+\.\d+\|\d+)", 0x800, 0x8);

        if (!ip_address) {
            spdlog::error("Failed to get ServerIp from DataModel");
            return false;
        }

        dumper::g_dumper.add_offset("DataModel", "ServerIP", *ip_address);

        const auto workspace_offset =
            process::Rtti::find(dumper::g_data_model_addr, "Workspace@RBX");

        if (!workspace_offset) {
            spdlog::error("Failed to get Workspace from DataModel");
            return false;
        }

        dumper::g_dumper.add_offset("DataModel", "Workspace", *workspace_offset);

        return true;
    }
} // namespace dumper::stages::data_model