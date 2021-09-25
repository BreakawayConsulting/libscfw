/*
 * Copyright 2021, Breakaway Consulting Pty. Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <scfw/types.h>
#include "rpc.h"

#define MU_TX_COUNT 4
#define MU_RX_COUNT 4
#define MAX_IPC_CH 8
#define MAX_ATTEMPTS 11770


/* page 8350*/
struct mu_regs {
    uint32_t tr[MU_TX_COUNT];
    uint32_t rr[MU_RX_COUNT];
    uint32_t sr;
    uint32_t cr;
};

static uintptr_t ipc_ch_reg_addr[MAX_IPC_CH];

static sc_err_t mu_send_word(volatile struct mu_regs *mu, uint8_t idx, uint32_t val)
{
    /* Poll until the register is free */
    for (unsigned i = 0; i < MAX_ATTEMPTS; i++) {
        uint32_t sr = mu->sr;
        if (((sr >> (23 - idx)) & 1) == 1) {
            mu->tr[idx] = val;
            return SC_ERR_NONE;
        }
    }

    return SC_ERR_FAIL;
}

static sc_err_t mu_recv_word(volatile struct mu_regs *mu, uint8_t idx, uint32_t *val)
{
    for (unsigned i = 0; i < MAX_ATTEMPTS; i++) {
        uint32_t sr = mu->sr;
        if (((sr >> (27 - idx)) & 1) == 1) {
            *val = mu->rr[idx];
            return SC_ERR_NONE;
        }
    }

    return SC_ERR_FAIL;
}

void sc_call_rpc(sc_ipc_t ipc, sc_rpc_msg_t *msg, sc_bool_t no_resp)
{
    sc_err_t err;
    uint32_t *d = (uint32_t*)msg;
    volatile struct mu_regs *mu;
    int i;

    if (ipc >= MAX_IPC_CH || ipc_ch_reg_addr[ipc] == 0) {
        /* No way to return an error, so just return immediately */
        return;
    }

    mu = (void *) ipc_ch_reg_addr[ipc];

    no_resp = no_resp;

    for (i = 0; i < msg->size; i++) {
        err = mu_send_word(mu, i % MU_TX_COUNT, d[i]);
        if (err != SC_ERR_NONE) {
            RPC_R8(msg) = err;
        }
    }

    if (no_resp) {
        return;
    }

    err = mu_recv_word(mu, 0, &d[0]);
    if (err != SC_ERR_NONE) {
        RPC_R8(msg) = err;
        return;
    }
    for (i = 1; i < msg->size; i++)  {
        err = mu_recv_word(mu, i % MU_RX_COUNT, &d[i]);
        if (err != SC_ERR_NONE) {
            RPC_R8(msg) = err;
            return;
        }
    }
}

sc_err_t sc_ipc_init(sc_ipc_t ipc, uintptr_t reg_addr)
{
    if (ipc >= MAX_IPC_CH) {
        return SC_ERR_UNAVAILABLE;
    }

     ipc_ch_reg_addr[ipc] = reg_addr;
}
