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
        $this->publisher = new EventPublisher();
        $this->manager = new MigrationManager($connection);
        $this->agent = new MigratorAgent($connection, $this->publisher);
    }

    public async function upgrade(): Awaitable<MigrationResult>
    {
        $migrations = $this->loader->loadUpMigration();

        await $this->manager->setUp();
        $diffMigrations = await $this->manager->diff($migrations);

        return await $this->upgradeScheme($diffMigrations);
    }

    public async function downgrade(): Awaitable<MigrationResult>
    {
        $appliedMigrations = await $this->manager->loadMigrations();
        $migrations = $this->loader->loadDownMigration($appliedMigrations);

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
