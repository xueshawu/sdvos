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
 * @file   src/include/arch/avr5/interrupt.h
 * @author Ye Li (liye@sdvos.org)
 * @brief  AVR5 Interrupt Management
 */
#ifndef _AVR5_INTERRUPT_H_
#define _AVR5_INTERRUPT_H_

#include <arch/avr5/task.h>
#include <sdvos.h>

/**
 * @def ISR_ATTR
 * @brief Blocking ISR
 *
 * With ISR_ATTR, the entry and exit code will be
 * generated by compiler (context will be saved on stack
 * instead of TCB). It will be more efficient since not
 * all processor context need to be saved. But no
 * scheduling could happen until the ISR returns.
 */
#define ISR_ATTR       __attribute__((signal, used, externally_visible))

/**
 * @def ISR_NOBLOCK
 * @brief Non blocking ISR
 *
 * Similar to ISR with ISR_ATTR. However, interrupt will
 * be enabled in the ISR to allow nested interrupt.
 */
#define ISR_NOBLOCK    __attribute__((interrupt))

/**
 * @def ISR_NAKED
 * @brief Naked ISR
 *
 * No code will be generated for either ISR entry or exit.
 * Wrappers that handles context save and restore have to
 * be provided along with the reti instruction at the end.
 */
#define ISR_NAKED      __attribute__((naked))

/**
 * @def ISR_CAT1
 * @brief Definition macro for Category 1 ISR
 *
 * Macro used for the definition of Category 1 ISR. System
 * services allowed in Category 1 ISR are:
 *
 * EnableAllInterrupts, DisableAllInterrupts
 * ResumeAllInterrupts, SuspendAllInterrupts
 * ResumeOSInterrupts, SuspendOSInterrupts
 *
 * @param[in] vector
 *   Interrupt vector
 */
#define ISR_CAT1(vector)          \
  void vector (void) ISR_ATTR;    \
  void vector##_impl (void);      \
  void vector (void) {            \
    NestedISRs++;                 \
    vector##_impl ();             \
    NestedISRs--;                 \
  }                               \
  void vector##_impl (void)


/**
 * @def ISR
 * @brief Definition macro for Category 2 ISR
 *
 * Macro used for the definition of Category 2 ISR. System
 * services allowed in Category 2 ISR are:
 *
 * ActivateTask, GetTaskID, GetTaskState, GetResource
 * ReleaseResource, SetEvent, GetEvent, GetAlarmBase
 * GetAlarm, SetRelAlarm, SetAbsAlarm, CancelAlarm
 * GetActiveApplicationMode, ShutdownOS
 *
 * @param[in] vector
 *   Interrupt vector
 */
#define ISR(vector)                          \
  void vector (void) ISR_ATTR ISR_NAKED;     \
  void vector##_impl (void);                 \
  void vector (void) {                       \
    IRQSaveContext ();                       \
    NestedISRs++;                            \
    vector##_impl ();                        \
    NestedISRs--;                            \
    CheckPreemption (PREEMPT_ISR);           \
    IRQRestoreContext ();                    \
    __asm__ volatile ("reti");               \
  }                                          \
  void vector##_impl (void)

/**
 * @def ArchEnableAllInterrupts
 * @brief Internal macro used by EnableAllInterrupts().
 */
#define ArchEnableAllInterrupts()  \
  __asm__ volatile ("sei")

/**
 * @def ArchDisableAllInterrupts
 * @brief Internal macro used by DisableAllInterrupts().
 */
#define ArchDisableAllInterrupts() \
  __asm__ volatile ("cli")

/*
 * Difficult to make Resume/SuspendAllInterrupts efficient
 * in AVR5. Let's just make it Enable/DisableAllInterrupts
 * for now. The important thing is, nested calls should be
 * supported.
 */

/**
 * @def ArchResumeAllInterrupts
 * @brief Internal macro used by ResumeAllInterrupts().
 */
#define ArchResumeAllInterrupts  ArchEnableAllInterrupts

/**
 * @def ArchSuspendAllInterrupts
 * @brief Internal macro used by SuspendAllInterrupts().
 */
#define ArchSuspendAllInterrupts  ArchDisableAllInterrupts

/*
 * Same as above. But if necessary, a stack structure can
 * be used to save PCICR and EIMSK registers. This assumes
 * OS interrupts only uses external interrupts.
 */

/**
 * @def ArchResumeOSInterrupts
 * @brief Internal macro used by ResumeOSInterrupts().
 */
#define ArchResumeOSInterrupts  ArchEnableAllInterrupts

/**
 * @def ArchSuspendOSInterrupts
 * @brief Internal macro used by SuspendOSInterrupts().
 */
#define ArchSuspendOSInterrupts  ArchDisableAllInterrupts

#endif

/* vi: set et ai sw=2 sts=2: */
