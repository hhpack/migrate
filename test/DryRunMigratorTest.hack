namespace HHPack\Migrate\Test;

use HHPack\Migrate\Test\Helper\Db\{WithDbName};
use HHPack\Migrate\{File, Migrator, DryRunMigrator};
use HHPack\Migrate\Migration\{MigrationNotFoundException};
use HHPack\Migrate\Migration\Loader\{SqlMigrationLoader};
use HHPack\Migrate\Database\{Connection};
use HHPack\Migrate\Logger\{ColoredLogger};

use type Facebook\HackTest\{HackTest, DataProvider};
use function Facebook\FBExpect\expect;

final class DryRunMigratorTest extends HackTest {
  use WithDbName;

  public async function beforeEachTestAsync(): Awaitable<void> {
    $conn = $this->currentConnection();
    \HH\Asio\join($conn->rawQuery("DROP TABLE IF EXISTS scheme_migrations"));
    \HH\Asio\join($conn->rawQuery("DROP TABLE IF EXISTS users"));
    \HH\Asio\join($conn->rawQuery("DROP TABLE IF EXISTS posts"));
  }

  public function provide(): vec<(Connection, Migrator, DryRunMigrator)> {
    $conn = $this->currentConnection();
    $path = File\absolute_path(__DIR__.'/sql/migrations');

    $loader = new SqlMigrationLoader($path);
    $migrator = new Migrator($loader, $conn, new ColoredLogger());
    $dryRunMigrator = new DryRunMigrator($loader, $conn, new ColoredLogger());

    return vec[tuple($conn, $migrator, $dryRunMigrator)];
  }

  <<DataProvider('provide')>>
  public function testUpgrade(
    Connection $conn,
    Migrator $migrator,
    DryRunMigrator $dryRunMigrator,
  ): void {
    $result = \HH\Asio\join($dryRunMigrator->upgrade());
    expect($result->resultCount())->toBeSame(2);
  }

  <<DataProvider('provide')>>
  public function testDowngradeByLastName(
    Connection $conn,
    Migrator $migrator,
    DryRunMigrator $dryRunMigrator,
  ): void {
    \HH\Asio\join($migrator->upgrade());

    $result = \HH\Asio\join(
      $dryRunMigrator->downgradeTo('20150824010439-create-users'),
    );

    expect($result->resultCount())->toBeSame(2);

    $posts = $result->at('20150825102100-create-posts');
    expect($posts->containsKey(0))->toBeTrue();

    $users = $result->at('20150824010439-create-users');
    expect($users->containsKey(0))->toBeTrue();
  }

  <<DataProvider('provide')>>
  public function testDowngradeByFirstName(
    Connection $conn,
    Migrator $migrator,
    DryRunMigrator $dryRunMigrator,
  ): void {
    \HH\Asio\join($migrator->upgrade());
    $result = \HH\Asio\join(
      $dryRunMigrator->downgradeTo('20150825102100-create-posts'),
    );

    expect($result->resultCount())->toBeSame(1);

    $posts = $result->at('20150825102100-create-posts');
    expect($posts->containsKey(0))->toBeTrue();

    expect($result->containsKey('20150824010439-create-users'))
      ->toBeFalse();
  }

  <<DataProvider('provide')>>
  public function testDowngradeAll(
    Connection $conn,
    Migrator $migrator,
    DryRunMigrator $dryRunMigrator,
  ): void {
    \HH\Asio\join($migrator->upgrade());
    $result = \HH\Asio\join($dryRunMigrator->downgrade());

    expect($result->resultCount())->toBeSame(2);

    $posts = $result->at('20150825102100-create-posts');
    expect($posts->containsKey(0))->toBeTrue();

    $users = $result->at('20150824010439-create-users');
    expect($users->containsKey(0))->toBeTrue();
  }
}
