#include "text_label.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace dumper::stages::text_label {

    struct TextLabelData {
        std::string name;
        uintptr_t address;
        control::client::TextLabelProperty props;
    };

    static auto get_text_label_data(const control::client::TextLabelPropertiesInfo& props)
        -> std::optional<std::vector<TextLabelData>> {
        std::vector<TextLabelData> text_label_data;

        const auto text_labels_folder = dumper::g_data_model.find_first_child("ReplicatedStorage")
                                            ->find_first_child("TestFramesGui")
                                            ->find_first_child("TextLabels");
        if (!text_labels_folder->is_valid()) {
            spdlog::error("Failed to find TextLabels Folder");
            return std::nullopt;
        }

        for (const auto& prop : props.text_labels) {
            const auto text_label = text_labels_folder->find_first_child(prop.name);
            if (!text_label->is_valid()) {
                spdlog::error("Failed to find text label: {}", prop.name);
                return std::nullopt;
            }

            TextLabelData data{
                .name = prop.name, .address = text_label->get_address(), .props = prop};

            text_label_data.push_back(data);
        }

        return text_label_data;
    }

    auto dump() -> bool {
        const auto text_label_props = control::client::g_client.get_textlabel_properties();
        if (!text_label_props) {
            spdlog::error("Failed to get text label properties from control server");
            return false;
        }

        if (text_label_props->text_labels.size() < 3) {
            spdlog::error("Not enough text labels found (need at least 3)");
            return false;
        }

        const auto text_labels = get_text_label_data(*text_label_props);
        if (!text_labels) {
            return false;
        }

        std::vector<uintptr_t> text_label_addrs;
        for (const auto& tl : *text_labels) {
            text_label_addrs.push_back(tl.address);
        }

        const auto font_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_label_addrs, [&](size_t i) { return (*text_labels)[i].props.font; }, 0x1000, 0x1);
        if (!font_offset) {
            spdlog::error("Failed to find Font offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "Font", *font_offset);

        const auto line_height_offset = process::helpers::find_offset_with_getter<float>(
            text_label_addrs, [&](size_t i) { return (*text_labels)[i].props.line_height; }, 0x1000,
            0x4);
        if (!line_height_offset) {
            spdlog::error("Failed to find LineHeight offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "LineHeight", *line_height_offset);

        const auto max_visible_graphemes_offset =
            process::helpers::find_offset_with_getter<int32_t>(
                text_label_addrs,
                [&](size_t i) { return (*text_labels)[i].props.max_visible_graphemes; }, 0x1000,
                0x4);
        if (!max_visible_graphemes_offset) {
            spdlog::error("Failed to find MaxVisibleGraphemes offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "MaxVisibleGraphemes",
                                    *max_visible_graphemes_offset);

        const auto rich_text_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_label_addrs, [&](size_t i) { return (*text_labels)[i].props.rich_text ? 1 : 0; },
            0x1000, 0x1);
        if (!rich_text_offset) {
            spdlog::error("Failed to find RichText offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "RichText", *rich_text_offset);

        const auto text_color_offset = process::helpers::find_vec_offset<glm::vec3>(
            text_label_addrs[0],
            glm::vec3((*text_labels)[0].props.text_color_r / 255.0f,
                      (*text_labels)[0].props.text_color_g / 255.0f,
                      (*text_labels)[0].props.text_color_b / 255.0f),
            0x1000, 0.01f, 0x4);
        if (!text_color_offset) {
            spdlog::error("Failed to find TextColor3 offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextColor3", *text_color_offset);

        const auto text_direction_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_label_addrs, [&](size_t i) { return (*text_labels)[i].props.text_direction; },
            0x1000, 0x1);
        if (!text_direction_offset) {
            spdlog::error("Failed to find TextDirection offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextDirection", *text_direction_offset);

        const auto text_fits_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_label_addrs, [&](size_t i) { return (*text_labels)[i].props.text_fits ? 1 : 0; },
            0x1000, 0x1);
        if (!text_fits_offset) {
            spdlog::error("Failed to find TextFits offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextFits", *text_fits_offset);

        const auto text_scaled_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_label_addrs, [&](size_t i) { return (*text_labels)[i].props.text_scaled ? 1 : 0; },
            0x1000, 0x1);
        if (!text_scaled_offset) {
            spdlog::error("Failed to find TextScaled offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextScaled", *text_scaled_offset);

        const auto text_size_offset = process::helpers::find_offset_with_getter<float>(
            text_label_addrs, [&](size_t i) { return (*text_labels)[i].props.text_size; }, 0x1000,
            0x4);
        if (!text_size_offset) {
            spdlog::error("Failed to find TextSize offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextSize", *text_size_offset);

        const auto text_stroke_color_offset = process::helpers::find_vec_offset<glm::vec3>(
            text_label_addrs[0],
            glm::vec3((*text_labels)[0].props.text_stroke_color_r / 255.0f,
                      (*text_labels)[0].props.text_stroke_color_g / 255.0f,
                      (*text_labels)[0].props.text_stroke_color_b / 255.0f),
            0x1000, 0.01f, 0x4);
        if (!text_stroke_color_offset) {
            spdlog::error("Failed to find TextStrokeColor3 offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextStrokeColor3", *text_stroke_color_offset);

        const auto text_stroke_transparency_offset =
            process::helpers::find_offset_with_getter<float>(
                text_label_addrs,
                [&](size_t i) { return (*text_labels)[i].props.text_stroke_transparency; }, 0x1000,
                0x4);
        if (!text_stroke_transparency_offset) {
            spdlog::error("Failed to find TextStrokeTransparency offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextStrokeTransparency",
                                    *text_stroke_transparency_offset);

        const auto text_transparency_offset = process::helpers::find_offset_with_getter<float>(
            text_label_addrs, [&](size_t i) { return (*text_labels)[i].props.text_transparency; },
            0x1000, 0x4);
        if (!text_transparency_offset) {
            spdlog::error("Failed to find TextTransparency offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextTransparency", *text_transparency_offset);

        const auto text_truncate_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_label_addrs, [&](size_t i) { return (*text_labels)[i].props.text_truncate; },
            0x1000, 0x1);
        if (!text_truncate_offset) {
            spdlog::error("Failed to find TextTruncate offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextTruncate", *text_truncate_offset);

        const auto text_wrapped_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_label_addrs,
            [&](size_t i) { return (*text_labels)[i].props.text_wrapped ? 1 : 0; }, 0x1000, 0x1);
        if (!text_wrapped_offset) {
            spdlog::error("Failed to find TextWrapped offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextWrapped", *text_wrapped_offset);

        const auto text_x_alignment_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_label_addrs, [&](size_t i) { return (*text_labels)[i].props.text_x_alignment; },
            0x1000, 0x1);
        if (!text_x_alignment_offset) {
            spdlog::error("Failed to find TextXAlignment offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextXAlignment", *text_x_alignment_offset);

        const auto text_y_alignment_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_label_addrs, [&](size_t i) { return (*text_labels)[i].props.text_y_alignment; },
            0x1000, 0x1);
        if (!text_y_alignment_offset) {
            spdlog::error("Failed to find TextYAlignment offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextLabel", "TextYAlignment", *text_y_alignment_offset);

        const auto& first_label = (*text_labels)[0];

        const auto text_offset = process::helpers::find_string_offset(
            first_label.address, first_label.props.text, 0x1000, 0x8, 0x256, true);
        if (text_offset) {
            dumper::g_dumper.add_offset("TextLabel", "Text", *text_offset);
        } else {
            spdlog::error("Failed to find Text offset");
            return false;
        }

        const auto content_text_offset = process::helpers::find_string_offset(
            first_label.address, first_label.props.content_text, 0x1000, 0x8, 0x256, true);
        if (content_text_offset) {
            dumper::g_dumper.add_offset("TextLabel", "ContentText", *content_text_offset);
        } else {
            spdlog::error("Failed to find ContentText offset");
            return false;
        }

        const auto localized_text_offset = process::helpers::find_string_offset(
            first_label.address, first_label.props.localized_text, 0x1000, 0x8, 0x256, true);
        if (localized_text_offset) {
            dumper::g_dumper.add_offset("TextLabel", "LocalizedText", *localized_text_offset);
        } else {
            spdlog::error("Failed to find LocalizedText offset");
            return false;
        }

        return true;
    }

} // namespace dumper::stages::text_label