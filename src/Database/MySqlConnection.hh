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

use AsyncMysqlClient;
use AsyncMysqlConnection;
use AsyncMysqlQueryResult;

final class MySqlConnection implements Connection
{

    public function __construct(
        private AsyncMysqlConnection $connection
    )
    {
    }

    public function escapeString(string $value): string
    {
        return $this->connection->escapeString($value);
    }

    public async function query(string $query): Awaitable<QueryResult>
    {
        $result = await $this->connection->query($query);
        $rows = $result->mapRowsTyped()
            ->map(($row) ==> $row->toImmMap())
            ->toImmVector();

        return new QueryResult($query, $rows, $result->startTime(), $result->endTime());
    }

    public function close(): void
    {
        $this->connection->close();
    }

    public function __destruct()
    {
        $this->close();
    }

    public static async function create(DSNString $value, string $username, string $password): Awaitable<this>
    {
        $dsn = DataSourceName::fromString($value);

        $port = $dsn->port();
        $port = $port === null ? 3306 : $port;

        $connection = await AsyncMysqlClient::connect(
            $dsn->host(),
            $port,
            $dsn->name(),
            $username,
            $password
        );

        return new static($connection);
    }

}
