<?hh //strict

namespace HHPack\Migrate\Test\Migration;

use HHPack\Migrate\Test\Helper\{Db};
use HHPack\Migrate\Migration\{MigrationManager};
use HHPack\Migrate\Database\{Connection};

use type Facebook\HackTest\{HackTest,DataProvider};
use function Facebook\FBExpect\expect;

final class MigrationManagerTest extends HackTest {
  <<__Memoize>>
  public function currentConnection(): Connection {
    return Db\connect();
  }

  public async function beforeEachTestAsync(): Awaitable<void> {
    $conn = $this->currentConnection();

    \HH\Asio\join($conn->rawQuery('DROP TABLE IF EXISTS scheme_migrations'));
  }

  public function provideManager(): vec<(MigrationManager, Connection)> {
    $conn = $this->currentConnection();
    $manager = new MigrationManager($conn);

    return vec[tuple($manager, $conn)];
  }

  <<DataProvider('provideManager')>>
  public function testSetup(MigrationManager $manager, Connection $conn): void {
    \HH\Asio\join($manager->setUp());
    $result =
      \HH\Asio\join($conn->rawQuery('SHOW CREATE TABLE scheme_migrations'));

    $rows = $result->rows();
    expect($rows->count())->toBeSame(1);
  }

  <<DataProvider('provideManager')>>
  public function testLoadMigrations(
    MigrationManager $manager,
    Connection $conn,
  ): void {
    \HH\Asio\join($manager->setUp());
    \HH\Asio\join(
      $conn
        ->rawQuery(
          "INSERT INTO scheme_migrations (name, run_at) VALUES ('20150824010439.up.sql', CURRENT_TIMESTAMP)",
        ),
    );
    \HH\Asio\join(
      $conn
        ->rawQuery(
          "INSERT INTO scheme_migrations (name, run_at) VALUES ('20150825102100.up.sql', CURRENT_TIMESTAMP)",
        ),
    );

    $result = \HH\Asio\join($manager->loadMigrations());

    \HH\Asio\join($conn->rawQuery("DELETE FROM scheme_migrations"));

    expect($result->count())->toBeSame(2);
  }
}
