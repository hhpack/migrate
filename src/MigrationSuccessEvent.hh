<?hh //strict

namespace hhpack\migrate;

use hhpack\publisher\Message;

final class MigrationSuccessEvent implements Message
{

    public function __construct(
        private QueryResult $result
    )
    {
    }

    public function sql(): string
    {
        return $this->result->sql();
    }

    public function startTime(): float
    {
        return $this->result->startTime();
    }

    public function endTime(): float
    {
        return $this->result->endTime();
    }

}