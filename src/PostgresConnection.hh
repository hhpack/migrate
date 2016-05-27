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

final class PostgresConnection implements Connection
{

    public function __construct(
        private resource $connection
    )
    {
    }

    public function escapeString(string $value): string
    {
        return pg_escape_string($this->connection, $value);
    }

    public async function query(string $query): Awaitable<QueryResult>
    {
        return new QueryResult($query, ImmVector {}, 0.0, 0.0);
    }

    public function close(): void
    {
        if (pg_connection_status($this->connection) !== PGSQL_CONNECTION_OK) {
            return;
        }
        pg_close($this->connection);
    }

    public function __destruct()
    {
        $this->close();
    }

}
