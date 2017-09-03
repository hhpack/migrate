<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Database;

/**
 * string value of data source name
 *
 *   mysql:dbname=test;port=3306
 */
type DSNString = string;

final class DatabaseClient
{

    public static async function createConnection(DSNString $value, string $username, string $password): Awaitable<Connection>
    {
        $dsn = DataSourceName::fromString($value);
        $connection = await MySqlConnection::createConnection($value, $username, $password);

        return $connection;
    }

    public static function createWithoutDbConnection(DatabaseServer $server, string $username, string $password): Connection
    {
        return MySqlConnection::createWithoutDbConnection($server, $username, $password);
    }

}
