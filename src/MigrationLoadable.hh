<?hh //strict

namespace hhpack\migrate;

interface MigrationLoadable
{
    public function loadUpMigration(): ImmVector<Migration>;
    public function loadDownMigration(): ImmVector<Migration>;
}
