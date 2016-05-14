<?hh //strict

namespace hhpack\migrate;

final class Migrator implements Migratable
{

    public function __construct(
        private MigrationLoadable $loader,
        private QueryProxy $proxy
    )
    {
    }

    public async function upgrade(): Awaitable<MigrationResult>
    {
        $results = Vector {};
        $migrations = $this->loader->loadUpMigration();

        foreach ($migrations->items() as $migration) {
            $result = await $migration->change($this->proxy);
            $results->add($result);
        }

        return new MigrationResult( $results->toImmVector() );
    }

    public async function downgrade(): Awaitable<MigrationResult>
    {
        $results = Vector {};
        $migrations = $this->loader->loadDownMigration();

        foreach ($migrations->items() as $migration) {
            $result = await $migration->change($this->proxy);
            $results->add($result);
        }

        return new MigrationResult( $results->toImmVector() );
    }

}
