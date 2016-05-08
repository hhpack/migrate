<?hh //strict

namespace hhpack\migrate;

use AsyncMysqlConnection;
use AsyncMysqlQueryResult;

final class MySqlConnection implements Connection<AsyncMysqlQueryResult>
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

    public async function query(string $query): Awaitable<AsyncMysqlQueryResult>
    {
        return await $this->connection->query($query);
    }

    public function close(): void
    {
        $this->connection->close();
    }

}
