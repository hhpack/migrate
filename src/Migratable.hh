<?hh //strict

namespace hhpack\migrate;

interface Migratable
{
    public function upgrade(): Awaitable<MigrationResult>;
    public function downgrade(): Awaitable<MigrationResult>;
}
