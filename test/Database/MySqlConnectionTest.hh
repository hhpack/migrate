<?hh //strict

namespace HHPack\Migrate\Test\Database;

use HHPack\Migrate\Test\Helper\{ Db };
use HHPack\Migrate\Database\{ Connection, MySqlConnection };
use HackPack\HackUnit\Contract\Assert;


final class MySqlConnectionTest
{
    public function __construct(
        private Connection $conn
    )
    {
    }

    <<SuiteProvider('Db')>>
    public static function create() : this
    {
        $conn = Db\connect();
        return new static($conn);
    }

    <<Test('Db')>>
    public function query(Assert $assert): void
    {
        $result = \HH\Asio\join($this->conn->rawQuery('show tables'));
        $assert->string($result->query())->is('show tables');
    }
}
