/**
 * This file is part of hhpack/migrate.
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate;

use HHPack\Migrate\Event\{EventPublisher};
use HHPack\Migrate\Migration\{
  MigrationManager,
  MigrationLoader,
  MigrationLogger,
  MigrationResult,
  MigrationNotFoundException,
};
use HHPack\Migrate\Database\{Connection, QueryResult};

final class DryRunMigrator implements Migratable {

  private MigratorAgent $agent;
  private MigrationManager $manager;
  private EventPublisher $publisher;

  public function __construct(
    private MigrationLoader $loader,
    Connection $connection,
    Logger $logger,
  ) {
    $this->publisher = new EventPublisher([new MigrationLogger($logger)]);
    $this->manager = new MigrationManager($connection);
    $this->agent = new MigratorAgent($connection, $this->publisher);
  }

  public async function upgrade(): Awaitable<MigrationResult> {
    $upgradeMigrations = await $this->selectUpgradeMigrations();

    await $this->publisher->migrationLoaded($upgradeMigrations);

    return $this->dryRun($upgradeMigrations);
  }

  public async function upgradeTo(
    MigrationName $name,
  ): Awaitable<MigrationResult> {
    $upgradeMigrations = await $this->selectUpgradeMigrations();

    $takeHandler = $migration ==> $migration->name() === $name;
    $targetUpgradeMigrations = $upgradeMigrations->takeWhile($takeHandler);

    await $this->publisher->migrationLoaded($targetUpgradeMigrations);

    return $this->dryRun($targetUpgradeMigrations);
  }

  public async function downgrade(): Awaitable<MigrationResult> {
    $migrations = await $this->selectDowngradeMigrations();

    await $this->publisher->migrationLoaded($migrations);

    return $this->dryRun($migrations);
  }

  public async function downgradeTo(
    MigrationName $name,
  ): Awaitable<MigrationResult> {
    $downgradeMigrations = await $this->selectDowngradeMigrations();
    $orderedMigrations = ($order, $migration) ==>
      Pair {$migration->name(), $order};

    $registry = ImmMap::fromItems(
      $downgradeMigrations->mapWithKey($orderedMigrations),
    );

    if (!$registry->containsKey($name)) {
      throw new MigrationNotFoundException("Migration $name is not found");
    }

    $orderIndex = $registry->at($name);
    $migrations = $downgradeMigrations->slice(0, $orderIndex + 1);

    await $this->publisher->migrationLoaded($migrations);

    return $this->dryRun($migrations);
  }

  private async function selectUpgradeMigrations(
  ): Awaitable<ImmVector<Migration>> {
    $migrations = $this->loader->loadUpgradeMigrations();
    await $this->manager->setUp();
    return await $this->manager->diff($migrations);
  }

  private async function selectDowngradeMigrations(
  ): Awaitable<ImmVector<Migration>> {
    $appliedMigrations = await $this->manager->loadMigrations();
    return $this->loader->loadDowngradeMigrations($appliedMigrations);
  }

  private function dryRun(ImmVector<Migration> $migrations): MigrationResult {
    $results = Map {};

    foreach ($migrations->items() as $migration) {
      $queryResults = $migration->queries()->toImmVector()->map(
        ($query) ==> {
          return new QueryResult([ImmMap {"query" => $query}], 0.0, 0.0);
        },
      );
      $results->set($migration->name(), $queryResults);
    }

    return new MigrationResult($results->toImmMap());
  }
}
