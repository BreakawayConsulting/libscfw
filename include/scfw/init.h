/*
 * Copyright 2021, Breakaway Consulting Pty. Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef SC_INIT_H
#define SC_INIT_H

#include <scfw/types.h>

/*!
 * This must be called prior to any API call.
 *
 * It is used to associated a specific IPC channel (specified
 * by `ipc`) with a specific set of message unit register values
 * (specified by `reg_addr`).
 *
 */
sc_err_t sc_ipc_init(sc_ipc_t ipc, uintptr_t reg_addr);

#endif