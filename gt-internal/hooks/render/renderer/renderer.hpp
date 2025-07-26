#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <stdio.h>
#include <stdint.h>

class c_renderer
{
public:
	void initialize();
	uintptr_t get_swap_buffers() const { return m_swap_buffers; };

private:
	uintptr_t m_swap_buffers;
};

extern c_renderer* g_renderer;