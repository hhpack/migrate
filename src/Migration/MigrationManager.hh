<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Migration;

use HHPack\Migrate\{MigrationName, Migration};
use HHPack\Migrate\Database\{Connection, QueryResult};
use HHPack\Migrate\Database\Query\{
  CreateMigrationsTableQuery,
  SelectMigrationsQuery,
  SaveMigrationQuery,
  RemoveMigrationQuery,
};

final class MigrationManager {

  const string TABLE_NAME = 'scheme_migrations';

  public function __construct(private Connection $connection) {}

  public async function setUp(): Awaitable<QueryResult> {
    return await $this->connection
      ->query(new CreateMigrationsTableQuery(static::TABLE_NAME));
  }

  public async function loadMigrations(): Awaitable<ImmSet<MigrationName>> {
    $result = await $this->connection
      ->query(new SelectMigrationsQuery(static::TABLE_NAME));
    $migrations = $result->pluck('name', ($v) ==> (string)$v);
    return $migrations;
  }

  public async function diff(
    ImmVector<Migration> $migrations,
  ): Awaitable<ImmVector<Migration>> {
    $appliedMigrations = await $this->loadMigrations();
    $filter =
      ($migration) ==> !$appliedMigrations->contains($migration->name());

    $diffMigrations = $migrations->filter($filter);

    return $diffMigrations;
  }

  public async function save(Migration $migration): Awaitable<QueryResult> {
    return await $this->connection
      ->query(new SaveMigrationQuery(static::TABLE_NAME, $migration->name()));
  }

  public async function remove(Migration $migration): Awaitable<QueryResult> {
    return await $this->connection
      ->query(new RemoveMigrationQuery(static::TABLE_NAME, $migration->name()));
  }

}
