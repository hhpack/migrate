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

    public async function diff(ImmVector<Migration> $migrations): Awaitable<ImmVector<Migration>>
    {
        $appliedMigrations = await $this->loadMigrations();
        $filter = ($migration) ==> !$appliedMigrations->contains($migration->version());

        $diffMigrations = $migrations->filter($filter);

        return $diffMigrations;
    }

    public async function save(Migration $migration): Awaitable<QueryResult>
    {
        $sql = sprintf(
            "INSERT INTO scheme_migrations (name) VALUES ('%s')",
            $this->connection->escapeString($migration->name())
        );

        return await $this->connection->query($sql);
    }

    public async function remove(Migration $migration): Awaitable<QueryResult>
    {
        $sql = sprintf(
            "DELETE FROM scheme_migrations WHERE name = '%s'",
            $this->connection->escapeString($migration->name())
        );

        return await $this->connection->query($sql);
    }

}
