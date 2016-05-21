<?hh //strict

namespace hhpack\migrate;

interface Migratable
{
    public function upgrade(?MigrationName $name = null): Awaitable<MigrationResult>;
    public function downgrade(MigrationName $name): Awaitable<MigrationResult>;
}
