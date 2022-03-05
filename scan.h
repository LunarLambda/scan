/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SCAN_H
#define SCAN_H

struct state
{
    int position;         // optind   / Index into begin[]
    int clump;            // nextchar / For dissecting -abc into -a -b -c...
    int option;           // optopt   / Contains character of unknown option
    const char *argument; // optarg   / Option argument, if present, or non-option argument. otherwise null.
};

int scan(const char **begin, const char **end, const char *optstring, struct state *state);

#endif /* SCAN_H */
