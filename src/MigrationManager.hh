<?hh //strict

namespace hhpack\migrate;

final class MigrationManager
{

    const LOG_TABLE = "CREATE TABLE IF NOT EXISTS scheme_migrations (`id` int(11) NOT NULL, `name` varchar(255), `run_at` datetime NOT NULL)";
    const MIGRATIONS_SQL = "SELECT name FROM scheme_migrations ORDER BY run_at DESC";

    public function __construct(
        private Connection $connection
    )
    {
    }

    public async function setUp(): Awaitable<QueryResult>
    {
        return await $this->connection->query(static::LOG_TABLE);
    }

    public async function loadMigrations(): Awaitable<ImmSet<string>>
    {
        $result = await $this->connection->query(static::MIGRATIONS_SQL);
        $migrations = $result->rows()
            ->map(($row) ==> (string) $row->at('name'))
            ->toImmSet();

        return $migrations;
    }

}
