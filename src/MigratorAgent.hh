<?hh //strict

namespace hhpack\migrate;

final class MigratorAgent implements QueryProxy
{

    public function __construct(
        private Connection $connection,
        private EventPublisher $publisher
    )
    {
    }

    public async function query(string $query): Awaitable<QueryResult>
    {
        await $this->publisher->migrationStart($query);
        $result = await $this->connection->query($query);
        await $this->publisher->migrationSuccess($result);

        return $result;
    }

}
