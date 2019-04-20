namespace HHPack\Migrate\Test\Database;

use HHPack\Migrate\Test\Helper\Db\{WithDbName};
use HHPack\Migrate\Database\{Connection, MySqlConnection};
use type Facebook\HackTest\{HackTest, DataProvider};
use function Facebook\FBExpect\expect;

final class MySqlConnectionTest extends HackTest {
  use WithDbName;

  public function provideConnection(): vec<(Connection)> {
    return vec[tuple($this->currentConnection())];
  }

  <<DataProvider('provideConnection')>>
  public function testQuery(Connection $conn): void {
    $result = \HH\Asio\join($conn->rawQuery('show tables'));
    expect($result->startTime() > 0.0)->toBeTrue();
  }
}
