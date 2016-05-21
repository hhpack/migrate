<?hh //strict

namespace hhpack\migrate;

final class Migrator implements Migratable
{

    private MigratorAgent $agent;
    private MigrationManager $manager;
    private EventPublisher $publisher;

    public function __construct(
        private MigrationLoadable $loader,
        Connection $connection
    )
    {
        $this->publisher = new EventPublisher([
            new MigrationLogger()
        ]);
        $this->manager = new MigrationManager($connection);
        $this->agent = new MigratorAgent($connection, $this->publisher);
    }

    public async function upgrade(?MigrationName $name = null): Awaitable<MigrationResult>
    {
        $migrations = $this->loader->loadUpMigration();

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
        $downgradeMigrations = $this->loader->loadDownMigration($appliedMigrations);

        $takeHandler = $migration ==> $migration->name() !== $name;
        $migrations = $downgradeMigrations->takeWhile($takeHandler);

        await $this->publisher->migrationLoaded($migrations);

        return await $this->downgradeScheme($migrations);
    }

    private async function downgradeScheme(ImmVector<Migration> $migrations): Awaitable<MigrationResult>
    {
        $results = Vector {};

        foreach ($migrations->items() as $migration) {
            $result = await $migration->change($this->agent);
            await $this->manager->remove($migration);
            $results->add($result);
        }

        return new MigrationResult( $results->toImmVector() );
    }

    private async function upgradeScheme(ImmVector<Migration> $migrations): Awaitable<MigrationResult>
    {
        $results = Vector {};

        foreach ($migrations->items() as $migration) {
            $result = await $migration->change($this->agent);
            await $this->manager->save($migration);
            $results->add($result);
        }

        return new MigrationResult( $results->toImmVector() );
    }

}
