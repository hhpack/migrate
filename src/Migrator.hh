<?hh //strict

namespace hhpack\migrate;

final class Migrator implements Migratable
{

    public function __construct(
        private Connection $connection
    )
    {
    }

    public async function up(): Awaitable<void>
    {
    }

    public async function down(): Awaitable<void>
    {
    }

}
