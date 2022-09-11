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
#pragma once
#include "Hooks.h"
#include "DataTable.hpp"

using RecvVarProxyFn = void(__cdecl*)(const CRecvProxyData*, void*, void*);

class recv_prop_hook
{
public:
	recv_prop_hook(sdk::RecvProp* prop, const RecvVarProxyFn proxy_fn) :
		m_property(prop),
		m_original_proxy_fn(prop->m_ProxyFn)
	{
		set_proxy_function(proxy_fn);
	}

	~recv_prop_hook()
	{
		m_property->m_ProxyFn = m_original_proxy_fn;
	}

	auto get_original_function() const -> RecvVarProxyFn
	{
		return m_original_proxy_fn;
	}

	auto set_proxy_function(const RecvVarProxyFn proxy_fn) const -> void
	{
		m_property->m_ProxyFn = proxy_fn;
	}

private:
	sdk::RecvProp* m_property;
	RecvVarProxyFn m_original_proxy_fn;
};
