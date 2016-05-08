<?hh //strict

namespace hhpack\migrate;

interface Migrator
{
    public function up(): Awaitable<void>;
    public function down(): Awaitable<void>;
}
