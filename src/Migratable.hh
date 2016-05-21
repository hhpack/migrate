<?hh //strict

namespace hhpack\migrate;

interface Migratable
{
    public function upgrade(): Awaitable<MigrationResult>;
    public function downgradeTo(MigrationName $name): Awaitable<MigrationResult>;
}
