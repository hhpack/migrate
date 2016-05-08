<?hh //strict

namespace hhpack\migrate;

interface Migratable
{
    public function up(): Awaitable<void>;
    public function down(): Awaitable<void>;
}
