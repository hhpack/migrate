<?hh //strict

namespace hhpack\migrate;

use hhpack\publisher\Message;

final class MigrationLoadedEvent implements Message
{

    public function __construct(
        private ImmVector<Migration> $migrations
    )
    {
    }

    public function queries(): ImmVector<string>
    {
        return $this->migrations->map(($migration) ==> $migration->query());
    }

    public function migrationCount(): int
    {
        return $this->migrations->count();
    }

}
