/*
 *         Standard Dependable Vehicle Operating System
 *
 * Copyright (C) 2015 Ye Li (liye@sdvos.org)
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file   src/include/cc.h
 * @author Ye Li (liye@sdvos.org)
 * @brief  Compiler Specific Macro Definitions
 */
#ifndef _CC_H_
#define _CC_H_

#ifdef __GNUC__

#define PACK_STRUCT __attribute__((packed))
#define ALIGNED(x)  __attribute__((aligned (x)))
#define NAKED       __attribute__((naked))
#define NOINLINE    __attribute__((noinline))
#define ASM         __asm__
#define CC_MB()     __asm__ volatile ("": : :"memory")

#elif _MSC_VER

#elif __BORLANDC__

#elif __MINGW32__

#endif

#endif

/* vi: set et ai sw=2 sts=2: */
