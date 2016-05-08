<?hh //strict

namespace hhpack\migrate;

final class QueryResult
{

    public function __construct(
        private string $sql,
        private float $startTime,
        private float $endTime
    )
    {
    }

    public function sql(): string
    {
        return $this->sql;
    }

    public function startTime(): float
    {
        return $this->startTime;
    }

    public function endTime(): float
    {
        return $this->endTime;
    }

}
