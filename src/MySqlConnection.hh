<?hh //strict

namespace hhpack\migrate;

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

}
