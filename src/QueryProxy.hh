<?hh //strict

namespace hhpack\migrate;

interface QueryProxy
{
    public function query(string $query): Awaitable<QueryResult>;
}
