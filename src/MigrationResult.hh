<?hh //strict

namespace hhpack\migrate;

final class MigrationResult
{

    public function __construct(
        private ImmVector<QueryResult> $results
    )
    {
    }

    public function results(): ImmVector<QueryResult>
    {
        return $this->results;
    }

    public function at(int $index): QueryResult
    {
        return $this->results->at($index);
    }

    public function resultCount(): int
    {
        return $this->results->count();
    }

}
