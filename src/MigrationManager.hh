<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate;

final class MigrationManager
{

    const LOG_TABLE = "CREATE TABLE IF NOT EXISTS scheme_migrations (`id` int(11) NOT NULL AUTO_INCREMENT, `name` varchar(255), `run_at` datetime NOT NULL, PRIMARY KEY (`id`))";
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

    public async function loadMigrations(): Awaitable<ImmSet<MigrationName>>
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
        $filter = ($migration) ==> !$appliedMigrations->contains($migration->name());

        $diffMigrations = $migrations->filter($filter);

        return $diffMigrations;
    }

    public async function save(Migration $migration): Awaitable<QueryResult>
    {
        $sql = sprintf(
            "INSERT INTO scheme_migrations (name, run_at) VALUES ('%s', CURRENT_TIMESTAMP)",
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
