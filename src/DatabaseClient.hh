<?hh //strict

/**
 * This file is part of hhpack\migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace hhpack\migrate;

final class DatabaseClient
{

    public static async function createConnection(DSNString $value, string $username, string $password): Awaitable<Connection>
    {
        $dsn = DataSourceName::fromString($value);

        if ($dsn->type() === DataSourceType::MySql) {
            $connection = await MySqlConnection::create($value, $username, $password);
        // default action
        } else {
            $connection = await MySqlConnection::create($value, $username, $password);
        }

        return $connection;
    }

}
