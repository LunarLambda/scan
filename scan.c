/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "scan.h"

int scan_option(const char **it, const char **end, const char *optstring, struct state *state)
{
    const char *arg = *it;
    const char optchar = arg[1 + state->clump];

    for (const char *c = optstring; *c != '\0'; c++ /* cheeky */)
    {
        if (optchar == *c) // Match
        {
            if (c[1] == ':') // Required argument
            {
                if (state->clump != 0) // Option with argument terminates clump
                {
                    state->position++;
                }

                if (arg[2 + state->clump] != '\0') // -xARGUMENT
                {
                    state->argument = &(arg[2 + state->clump]);
                    state->clump = 0;
                    return optchar;
                }

                // We will no longer be indexing into arg
                state->clump = 0;

                if (it + 1 == end || it[1][0] == '-') // No argument for us, check if it's optional
                {
                    if (c[2] == ':') // Yep, all good
                    {
                        return optchar;
                    }
                    else // Nope, error
                    {
                        state->option = optchar;
                        state->argument = arg;
                        return ':';
                    }
                }

                // -x ARGUMENT
                state->position++;
                state->argument = it[1];
                return optchar;
            }
            else // No argument
            {
                if (arg[2 + state->clump] != '\0') // Start/continue clump if there is one
                {
                    state->clump++;
                }
                else if (state->clump != 0) // End clump if we have one
                {
                    state->clump = 0;
                    state->position++;
                }

                // -x
                return optchar;
            }
        }
    }

    // No match
    if (state->clump != 0);
    {
        state->clump = 0;
        state->position++;
    }

    state->option = optchar;
    state->argument = arg;
    return '?';
}

int scan(const char **begin, const char **end, const char *optstring, struct state *state)
{
    const char **it = &(begin[state->position]);
    const char *arg = *it;

    if (it != end)
    {
        if (state->clump != 0) // If we found a clump last round, keep parsing it
        {
            return scan_option(it, end, optstring, state);
        }

        state->position++;      // Bump position for next iteration
        state->option = 0;
        state->argument = 0;

        if (arg[0] == '-')
        {
            switch(arg[1])
            {
                case '\0': // -, non-option argument
                {
                    state->argument = arg;
                    return 1;
                }

                case '-': // --, stop parsing
                {
                    return -1;
                }

                default: // Option
                {
                    int res = scan_option(it, end, optstring, state);

                    if (state->clump != 0) // If we have a clump, rewind the iterator so we can continue next round
                    {
                        state->position--;
                    }

                    return res;
                }
            }
        }
        else // Non-option
        {
            state->argument = arg;
            return 1;
        }
    }

    return -1; // We're done
}
