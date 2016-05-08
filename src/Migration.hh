<?hh //strict

namespace hhpack\migrate;

interface Migration
{
    public function change(QueryProxy $proxy): Awaitable<QueryResult>;
}
