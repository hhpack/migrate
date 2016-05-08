<?hh //strict

namespace hhpack\migrate;

final class MigratorAgent implements QueryProxy
{

    public function __construct(
        private Connection $connection
    )
    {
    }

    public async function query(string $query): Awaitable<QueryResult>
    {
        return await $this->connection->query($query);
    }

}
