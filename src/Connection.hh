<?hh //strict

namespace hhpack\migrate;

interface Connection
{
    public function query(string $query): Awaitable<QueryResult>;
    public function escapeString(string $value): string;
    public function close(): void;
}
