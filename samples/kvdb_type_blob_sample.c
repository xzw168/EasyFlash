/*
 * This file is part of the EasyFlash Library.
 *
 * Copyright (c) 2020, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Key-Value Database blob type KV feature samples source file.
 * Created on: 2019-05-05
 */

#include <easyflash.h>

static uint32_t boot_count = 0;
static time_t boot_time[10] = {0, 1, 2, 3};
/* default KV nodes */
static struct ef_default_kv_node default_kv_table[] = {
        {"username", "armink", 0}, /* string KV */
        {"password", "123456", 0}, /* string KV */
        {"boot_count", &boot_count, sizeof(boot_count)}, /* int type KV */
        {"boot_time", &boot_time, sizeof(boot_time)},    /* int array type KV */
};
/* KVDB object */
static struct ef_kvdb kvdb = { 0 };

static void lock(ef_db_t db)
{
    /* YOUR CODE HERE */
}

static void unlock(ef_db_t db)
{
    /* YOUR CODE HERE */
}

void kvdb_type_blob_sample(void)
{
    EfErrCode result;
    struct ef_blob blob;

    { /* database initialization */
        struct ef_default_kv default_kv;

        default_kv.kvs = default_kv_table;
        default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
        /* set the lock and unlock function if you want */
        ef_db_lock_set((ef_db_t)&kvdb, lock, unlock);
        /* Key-Value database initialization
         *
         *       &kvdb: database object
         *       "env": database name
         *  "ef_kvdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         * &default_kv: The default KV nodes. It will auto add to KVDB when first initialize successfully.
         *        NULL: The user data if you need, now is empty.
         */
        result = ef_kvdb_init(&kvdb, "env", "ef_kvdb1", &default_kv, NULL);

        if (result != EF_NO_ERR) {
            return;
        }
    }

    { /* CREATE new Key-Value */
        int temp_data = 36;

        /* It will create new KV node when "temp" KV not in database.
         * ef_blob_make: It's a blob make function, and it will return the blob when make finish.
         */
        ef_kv_set_blob(&kvdb, "temp", ef_blob_make(&blob, &temp_data, sizeof(temp_data)));
    }

    { /* GET the KV value */
        int temp_data = 0;

        /* get the "temp" KV value */
        ef_kv_get_blob(&kvdb, "temp", ef_blob_make(&blob, &temp_data, sizeof(temp_data)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            EF_PRINT("temp_data: %d\n", temp_data);
        }
    }

    { /* CHANGE the KV value */
        int temp_data = 38;

        /* change the "temp" KV's value to 38.1 */
        ef_kv_set_blob(&kvdb, "temp", ef_blob_make(&blob, &temp_data, sizeof(temp_data)));
    }

    { /* DELETE the KV by name */
        ef_kv_del(&kvdb, "temp");
    }
}
