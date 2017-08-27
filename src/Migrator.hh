<?hh //strict

/**
 * This file is part of hhpack/migrate.
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate;

use HHPack\Migrate\Event\{ EventPublisher };
use HHPack\Migrate\Migration\{ MigrationManager, MigrationLoader, MigrationLogger, MigrationResult };
use HHPack\Migrate\Database\{ Connection };

final class Migrator implements Migratable
{

    private MigratorAgent $agent;
    private MigrationManager $manager;
    private EventPublisher $publisher;

    public function __construct(
        private MigrationLoader $loader,
        Connection $connection,
        Logger $logger
    )
    {
        $this->publisher = new EventPublisher([
            new MigrationLogger($logger)
        ]);
        $this->manager = new MigrationManager($connection);
        $this->agent = new MigratorAgent($connection, $this->publisher);
    }

    public async function upgrade(?MigrationName $name = null): Awaitable<MigrationResult>
    {
        $migrations = $this->loader->loadUpgradeMigrations();

        await $this->manager->setUp();
        $diffMigrations = await $this->manager->diff($migrations);
        $takeHandler = $migration ==> true;

        if ($name !== null) {
            $takeHandler = $migration ==> $migration->name() === $name;
        }

        $upgradeMigrations = $diffMigrations->takeWhile($takeHandler);

        await $this->publisher->migrationLoaded($upgradeMigrations);

        return await $this->upgradeScheme($upgradeMigrations);
    }

    public async function downgrade(MigrationName $name): Awaitable<MigrationResult>
    {
        $appliedMigrations = await $this->manager->loadMigrations();
        $downgradeMigrations = $this->loader->loadDowngradeMigrations($appliedMigrations);

        $takedMigrations = Vector {};
        $takeHandler = $migration ==> $migration->name() !== $name;

        foreach ($downgradeMigrations->lazy() as $migration) {
            if ($takeHandler($migration)) {
                $takedMigrations->add($migration);
            } else {
                $takedMigrations->add($migration);
                break;
            }
        }
        $migrations = $takedMigrations->immutable();

        await $this->publisher->migrationLoaded($migrations);

        return await $this->downgradeScheme($migrations);
    }

    private async function downgradeScheme(ImmVector<Migration> $migrations): Awaitable<MigrationResult>
    {
        $results = Map {};

        foreach ($migrations->items() as $migration) {
            $changeResults = await $migration->change($this->agent);
            await $this->manager->remove($migration);
            $results->set($migration->name(), $changeResults);
        }

        return new MigrationResult( $results->toImmMap() );
    }

    private async function upgradeScheme(ImmVector<Migration> $migrations): Awaitable<MigrationResult>
    {
        $results = Map {};

        foreach ($migrations->items() as $migration) {
            $changeResults = await $migration->change($this->agent);
            await $this->manager->save($migration);
            $results->set($migration->name(), $changeResults);
        }

        return new MigrationResult( $results->toImmMap() );
    }

}
