<?hh //strict

namespace hhpack\migrate;

final class MigrationLoader implements MigrationLoadable
{

    public function __construct(
        private MigrationLoadable $loader
    )
    {
    }

    public function loadUpMigration(): ImmVector<Migration>
    {
        return $this->loader->loadUpMigration();
    }

    public function loadDownMigration(): ImmVector<Migration>
    {
        return $this->loader->loadDownMigration();
    }

}
