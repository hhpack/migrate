<?hh //strict

namespace hhpack\migrate;

use hhpack\publisher\Message;

final class MigrationStartEvent implements Message
{

    public function __construct()
    {
    }

}
