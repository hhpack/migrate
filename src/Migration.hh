<?hh //strict

namespace hhpack\migrate;

interface Migration
{
    public function up(): Awaitable<void>;
    public function down(): Awaitable<void>;
}
