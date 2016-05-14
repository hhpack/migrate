<?hh //strict

namespace hhpack\migrate;

interface Migration
{
    public function version(): string;
    public function change(QueryProxy $proxy): Awaitable<QueryResult>;
}
