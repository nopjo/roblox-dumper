#include "text_button.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace dumper::stages::text_button {

    struct TextButtonData {
        std::string name;
        uintptr_t address;
        control::client::TextButtonProperty props;
    };

    static auto get_text_button_data(const control::client::TextButtonPropertiesInfo& props)
        -> std::optional<std::vector<TextButtonData>> {
        std::vector<TextButtonData> text_button_data;

        const auto text_buttons_folder = dumper::g_data_model.find_first_child("ReplicatedStorage")
                                             ->find_first_child("TestFramesGui")
                                             ->find_first_child("TextButtons");
        if (!text_buttons_folder->is_valid()) {
            spdlog::error("Failed to find TextButtons folder in TestFramesGui");
            return std::nullopt;
        }

        for (const auto& prop : props.text_buttons) {
            const auto text_button = text_buttons_folder->find_first_child(prop.name);
            if (!text_button->is_valid()) {
                spdlog::error("Failed to find text button: {}", prop.name);
                return std::nullopt;
            }

            TextButtonData data{
                .name = prop.name, .address = text_button->get_address(), .props = prop};

            text_button_data.push_back(data);
        }

        return text_button_data;
    }

    auto dump() -> bool {
        const auto text_button_props = control::client::g_client.get_textbutton_properties();
        if (!text_button_props) {
            spdlog::error("Failed to get text button properties from control server");
            return false;
        }

        if (text_button_props->text_buttons.size() < 3) {
            spdlog::error("Not enough text buttons found (need at least 3)");
            return false;
        }

        const auto text_buttons = get_text_button_data(*text_button_props);
        if (!text_buttons) {
            return false;
        }

        std::vector<uintptr_t> text_button_addrs;
        for (const auto& tb : *text_buttons) {
            text_button_addrs.push_back(tb.address);
        }

        const auto font_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.font; }, 0x1200,
            0x1);
        if (!font_offset) {
            spdlog::error("Failed to find Font offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "Font", *font_offset);

        const auto line_height_offset = process::helpers::find_offset_with_getter<float>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.line_height; },
            0x1200, 0x4);
        if (!line_height_offset) {
            spdlog::error("Failed to find LineHeight offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "LineHeight", *line_height_offset);

        const auto max_visible_graphemes_offset =
            process::helpers::find_offset_with_getter<int32_t>(
                text_button_addrs,
                [&](size_t i) { return (*text_buttons)[i].props.max_visible_graphemes; }, 0x1200,
                0x4);
        if (!max_visible_graphemes_offset) {
            spdlog::error("Failed to find MaxVisibleGraphemes offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "MaxVisibleGraphemes",
                                    *max_visible_graphemes_offset);

        const auto rich_text_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.rich_text ? 1 : 0; },
            0x1200, 0x1);
        if (!rich_text_offset) {
            spdlog::error("Failed to find RichText offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "RichText", *rich_text_offset);

        const auto text_color_offset = process::helpers::find_vec_offset<glm::vec3>(
            text_button_addrs[0],
            glm::vec3((*text_buttons)[0].props.text_color_r / 255.0f,
                      (*text_buttons)[0].props.text_color_g / 255.0f,
                      (*text_buttons)[0].props.text_color_b / 255.0f),
            0x1200, 0.01f, 0x4);
        if (!text_color_offset) {
            spdlog::error("Failed to find TextColor3 offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextColor3", *text_color_offset);

        const auto text_direction_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.text_direction; },
            0x1200, 0x1);
        if (!text_direction_offset) {
            spdlog::error("Failed to find TextDirection offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextDirection", *text_direction_offset);

        const auto text_fits_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.text_fits ? 1 : 0; },
            0x1200, 0x1);
        if (!text_fits_offset) {
            spdlog::error("Failed to find TextFits offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextFits", *text_fits_offset);

        const auto text_scaled_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs,
            [&](size_t i) { return (*text_buttons)[i].props.text_scaled ? 1 : 0; }, 0x1200, 0x1);
        if (!text_scaled_offset) {
            spdlog::error("Failed to find TextScaled offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextScaled", *text_scaled_offset);

        const auto text_size_offset = process::helpers::find_offset_with_getter<float>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.text_size; }, 0x1200,
            0x4);
        if (!text_size_offset) {
            spdlog::error("Failed to find TextSize offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextSize", *text_size_offset);

        const auto text_stroke_color_offset = process::helpers::find_vec_offset<glm::vec3>(
            text_button_addrs[0],
            glm::vec3((*text_buttons)[0].props.text_stroke_color_r / 255.0f,
                      (*text_buttons)[0].props.text_stroke_color_g / 255.0f,
                      (*text_buttons)[0].props.text_stroke_color_b / 255.0f),
            0x1200, 0.01f, 0x4);
        if (!text_stroke_color_offset) {
            spdlog::error("Failed to find TextStrokeColor3 offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextStrokeColor3", *text_stroke_color_offset);

        const auto text_stroke_transparency_offset =
            process::helpers::find_offset_with_getter<float>(
                text_button_addrs,
                [&](size_t i) { return (*text_buttons)[i].props.text_stroke_transparency; }, 0x1200,
                0x4);
        if (!text_stroke_transparency_offset) {
            spdlog::error("Failed to find TextStrokeTransparency offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextStrokeTransparency",
                                    *text_stroke_transparency_offset);

        const auto text_transparency_offset = process::helpers::find_offset_with_getter<float>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.text_transparency; },
            0x1200, 0x4);
        if (!text_transparency_offset) {
            spdlog::error("Failed to find TextTransparency offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextTransparency", *text_transparency_offset);

        const auto text_truncate_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.text_truncate; },
            0x1200, 0x1);
        if (!text_truncate_offset) {
            spdlog::error("Failed to find TextTruncate offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextTruncate", *text_truncate_offset);

        const auto text_wrapped_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs,
            [&](size_t i) { return (*text_buttons)[i].props.text_wrapped ? 1 : 0; }, 0x1200, 0x1);
        if (!text_wrapped_offset) {
            spdlog::error("Failed to find TextWrapped offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextWrapped", *text_wrapped_offset);

        const auto text_x_alignment_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.text_x_alignment; },
            0x1200, 0x1);
        if (!text_x_alignment_offset) {
            spdlog::error("Failed to find TextXAlignment offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextXAlignment", *text_x_alignment_offset);

        const auto text_y_alignment_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.text_y_alignment; },
            0x1200, 0x1);
        if (!text_y_alignment_offset) {
            spdlog::error("Failed to find TextYAlignment offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "TextYAlignment", *text_y_alignment_offset);

        const auto auto_button_color_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs,
            [&](size_t i) { return (*text_buttons)[i].props.auto_button_color ? 1 : 0; }, 0x1200,
            0x1);
        if (!auto_button_color_offset) {
            spdlog::error("Failed to find AutoButtonColor offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "AutoButtonColor", *auto_button_color_offset);

        const auto modal_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.modal ? 1 : 0; },
            0x1200, 0x1);
        if (!modal_offset) {
            spdlog::error("Failed to find Modal offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "Modal", *modal_offset);

        const auto selected_offset = process::helpers::find_offset_with_getter<uint8_t>(
            text_button_addrs, [&](size_t i) { return (*text_buttons)[i].props.selected ? 1 : 0; },
            0x1200, 0x1);
        if (!selected_offset) {
            spdlog::error("Failed to find Selected offset");
            return false;
        }
        dumper::g_dumper.add_offset("TextButton", "Selected", *selected_offset);

        const auto& first_button = (*text_buttons)[0];

        const auto text_offset = process::helpers::find_string_offset(
            first_button.address, first_button.props.text, 0x1200, 0x8, 0x256, true);
        if (text_offset) {
            dumper::g_dumper.add_offset("TextButton", "Text", *text_offset);
        } else {
            spdlog::error("Failed to find Text offset");
            return false;
        }

        const auto content_text_offset = process::helpers::find_string_offset(
            first_button.address, first_button.props.content_text, 0x1200, 0x8, 0x256, true);
        if (content_text_offset) {
            dumper::g_dumper.add_offset("TextButton", "ContentText", *content_text_offset);
        } else {
            spdlog::error("Failed to find ContentText offset");
            return false;
        }

        const auto localized_text_offset = process::helpers::find_string_offset(
            first_button.address, first_button.props.localized_text, 0x1200, 0x8, 0x256, true);
        if (localized_text_offset) {
            dumper::g_dumper.add_offset("TextButton", "LocalizedText", *localized_text_offset);
        } else {
            spdlog::error("Failed to find LocalizedText offset");
            return false;
        }

        return true;
    }

} // namespace dumper::stages::text_button