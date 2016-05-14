<?hh //strict

namespace hhpack\migrate;

final class Migrator implements Migratable
{

    private MigratorAgent $agent;
    private EventPublisher $publisher;

    public function __construct(
        private MigrationLoadable $loader,
        Connection $connection
    )
    {
        $this->publisher = new EventPublisher();
        $this->agent = new MigratorAgent($connection, $this->publisher);
    }

    public async function upgrade(): Awaitable<MigrationResult>
    {
        $results = Vector {};
        $migrations = $this->loader->loadUpMigration();

        foreach ($migrations->items() as $migration) {
            $result = await $migration->change($this->agent);
            $results->add($result);
        }

        return new MigrationResult( $results->toImmVector() );
    }

    public async function downgrade(): Awaitable<MigrationResult>
    {
        $results = Vector {};
        $migrations = $this->loader->loadDownMigration();

        foreach ($migrations->items() as $migration) {
            $result = await $migration->change($this->agent);
            $results->add($result);
        }

        return new MigrationResult( $results->toImmVector() );
    }

}
