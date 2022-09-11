#pragma once
/* This file is part of nSkinz by namazso, licensed under the MIT license:
*
* MIT License
*
* Copyright (c) namazso 2018
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "Hooks.h"
#include "kitparser.hpp"


std::vector<game_data::paint_kit> game_data::skin_kits;
std::vector<game_data::paint_kit> game_data::glove_kits;
std::vector<game_data::paint_kit> game_data::sticker_kits;



auto game_data::initialize_kits() -> void
{
	// platform::get_export("vstdlib.dll", "V_UCS2ToUTF8")
	const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA(crypt_str("vstdlib.dll")), crypt_str("V_UCS2ToUTF8"))));

	// Search the relative calls

	// call    ItemSystem
	// push    dword ptr [esi+0Ch]
	// lea     ecx, [eax+4]
	// call    CEconItemSchema::GetPaintKitDefinition

	const auto sig_address = csgo->Utils.FindPatternIDA(GetModuleHandleA(crypt_str("client.dll")), crypt_str("E8 ? ? ? ? FF 76 0C 8D 48 04 E8"));
		
	// Skip the opcode, read rel32 address
	const auto item_system_offset = *reinterpret_cast<std::int32_t*>(sig_address + 1);

	// Add the offset to the end of the instruction
	const auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem * (*)()>(sig_address + 5 + item_system_offset);

	// Skip VTable, first member variable of ItemSystem is ItemSchema
	const auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(std::uintptr_t(item_system_fn()) + sizeof(void*));

	// Dump paint kits
	{
		/*
		// Skip the instructions between, skip the opcode, read rel32 address
		const auto get_paint_kit_definition_offset = *reinterpret_cast<std::int32_t*>(sig_address + 11 + 1);

		// Add the offset to the end of the instruction
		const auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit * (__thiscall*)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);

		// The last offset is start_element, we need that

		// push    ebp
		// mov     ebp, esp
		// sub     esp, 0Ch
		// mov     eax, [ecx+298h]

		// Skip instructions, skip opcode, read offset
		const auto start_element_offset = *reinterpret_cast<std::intptr_t*>(std::uintptr_t(get_paint_kit_definition_fn) + 8 + 2);

		// Calculate head base from start_element's offset
		const auto head_offset = start_element_offset - 12;

		const auto map_head = reinterpret_cast<Head_t<int, CPaintKit*>*>(std::uintptr_t(item_schema) + head_offset);

		for (auto i = 0; i <= map_head->last_element; ++i)
		{
			const auto paint_kit = map_head->memory[i].value;

			if (paint_kit->id == 9001)
				continue;

			const auto wide_name = interfaces.localize->Find(paint_kit->item_name.buffer + 1);
			char name[256];
			V_UCS2ToUTF8(wide_name, name, sizeof(name));

			if (paint_kit->id < 10000)
				game_data::skin_kits.push_back({ paint_kit->id, name });
			else
				game_data::glove_kits.push_back({ paint_kit->id, name });
		}

		*/

		const auto& paintkitMap = itemSystem()->getItemSchema()->paintKits;
		int paintkitcount = 0;
		for (const auto& node : paintkitMap) {
			const auto pKit = node.value;

			if (pKit->id == 9001)
				continue;

			const auto wide_name = interfaces.localize->Find(pKit->item_name.buffer + 1);
			char name[256];
			V_UCS2ToUTF8(wide_name, name, sizeof(name));

			if(pKit->id < 10000)
				game_data::skin_kits.push_back({ pKit->id, name, pKit });
			else
				game_data::glove_kits.push_back({ pKit->id, name, pKit });


			paintkitcount++;
		}
		


		std::sort(game_data::skin_kits.begin(), game_data::skin_kits.end());
		std::sort(game_data::glove_kits.begin(), game_data::glove_kits.end());
	}

	// Dump sticker kits
	{
		/*
		const auto sticker_sig = csgo->Utils.FindPatternIDA(GetModuleHandleA(crypt_str("client.dll")), crypt_str("53 8D 48 04 E8 ? ? ? ? 8B 4D 10")) + 4;
			
		// Skip the opcode, read rel32 address
		const auto get_sticker_kit_definition_offset = *reinterpret_cast<std::intptr_t*>(sticker_sig + 1);

		// Add the offset to the end of the instruction
		const auto get_sticker_kit_definition_fn = reinterpret_cast<CPaintKit * (__thiscall*)(CCStrike15ItemSchema*, int)>(sticker_sig + 5 + get_sticker_kit_definition_offset);

		// The last offset is head_element, we need that

		//	push    ebp
		//	mov     ebp, esp
		//	push    ebx
		//	push    esi
		//	push    edi
		//	mov     edi, ecx
		//	mov     eax, [edi + 2BCh]

		// Skip instructions, skip opcode, read offset
		const auto start_element_offset = *reinterpret_cast<intptr_t*>(std::uintptr_t(get_sticker_kit_definition_fn) + 8 + 2);

		// Calculate head base from start_element's offset
		const auto head_offset = start_element_offset - 12;

		const auto map_head = reinterpret_cast<Head_t<int, CStickerKit*>*>(std::uintptr_t(item_schema) + head_offset);

		for (auto i = 0; i <= map_head->last_element; ++i)
		{
			const auto sticker_kit = map_head->memory[i].value;

			char sticker_name_if_valve_fucked_up_their_translations[64];

			auto sticker_name_ptr = sticker_kit->item_name.buffer + 1;

			//if it is SprayKit - ignore, it can`t be installed same as StickerKit
			if (sticker_name_ptr[1] == 'p')
				continue;

			if (strstr(sticker_name_ptr, "StickerKit_dhw2014_dignitas"))
			{
				strcpy_s(sticker_name_if_valve_fucked_up_their_translations, "StickerKit_dhw2014_teamdignitas");
				strcat_s(sticker_name_if_valve_fucked_up_their_translations, sticker_name_ptr + 27);
				sticker_name_ptr = sticker_name_if_valve_fucked_up_their_translations;
			}

			const auto wide_name = interfaces.localize->Find(sticker_name_ptr);
			char name[256];
			V_UCS2ToUTF8(wide_name, name, sizeof(name));

			game_data::sticker_kits.push_back({ sticker_kit->id, name, sticker_kit });
		}
		*/

		const auto& stickerMap = itemSystem()->getItemSchema()->stickerKits;
		
		for (const auto& node : stickerMap) {
			const auto stickerkit = node.value;

			char sticker_name_if_valve_fucked_up_their_translations[64];

			auto sticker_name_ptr = stickerkit->item_name.buffer + 1;

			//if it is SprayKit - ignore, it can`t be installed same as StickerKit
			if (sticker_name_ptr[1] == 'p')
				continue;

			if (strstr(sticker_name_ptr, crypt_str("StickerKit_dhw2014_dignitas")))
			{
				strcpy_s(sticker_name_if_valve_fucked_up_their_translations, crypt_str("StickerKit_dhw2014_teamdignitas"));
				strcat_s(sticker_name_if_valve_fucked_up_their_translations, sticker_name_ptr + 27);
				sticker_name_ptr = sticker_name_if_valve_fucked_up_their_translations;
			}

			const auto wide_name = interfaces.localize->Find(sticker_name_ptr);
			char name[256];
			V_UCS2ToUTF8(wide_name, name, sizeof(name));

			game_data::sticker_kits.push_back(game_data::paint_kit{ stickerkit->id, std::string(name), (CPaintKit*)stickerkit });
			
		}

		std::sort(game_data::sticker_kits.begin(), game_data::sticker_kits.end());

		game_data::sticker_kits.insert(game_data::sticker_kits.begin(), { 0, crypt_str("None"), nullptr });
	}
}
