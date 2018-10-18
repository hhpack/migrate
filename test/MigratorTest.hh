<?hh //strict

namespace HHPack\Migrate\Test;

use HHPack\Migrate\Test\Helper\{Db};
use HHPack\Migrate\{File, Migrator};
use HHPack\Migrate\Migration\{MigrationNotFoundException};
use HHPack\Migrate\Migration\Loader\{SqlMigrationLoader};
use HHPack\Migrate\Database\{Connection};
use HHPack\Migrate\Logger\{ColoredLogger};

use type Facebook\HackTest\{HackTest,DataProvider};
use function Facebook\FBExpect\expect;

final class MigratorTest extends HackTest {
  public async function beforeEachTestAsync(): Awaitable<void> {
    $conn = Db\connect();

    await $conn->rawQuery("DROP TABLE IF EXISTS scheme_migrations");
    await $conn->rawQuery("DROP TABLE IF EXISTS users");
    await $conn->rawQuery("DROP TABLE IF EXISTS posts");
  }

  public function provideCreate(): vec<(Connection, Migrator)> {
    $conn = Db\connect();
    $path = File\absolute_path(__DIR__.'/sql/migrations');

    $loader = new SqlMigrationLoader($path);
    $migrator = new Migrator($loader, $conn, new ColoredLogger());

    return vec[tuple($conn, $migrator)];
  }

  <<DataProvider('provideCreate')>>
  public function testUpgrade(Connection $conn, Migrator $migrator): void {
    $result = \HH\Asio\join($migrator->upgrade());
    expect($result->resultCount())->toBeSame(2);
  }

  <<DataProvider('provideCreate')>>
  public function testDowngradeByLastName(
    Connection $conn,
    Migrator $migrator,
  ): void {
    \HH\Asio\join($migrator->upgrade());
    $result =
      \HH\Asio\join($migrator->downgradeTo('20150824010439-create-users'));

    expect($result->resultCount())->toBeSame(2);

    $posts = $result->at('20150825102100-create-posts');
    expect($posts->containsKey(0))->toBeTrue();

    $users = $result->at('20150824010439-create-users');
    expect($users->containsKey(0))->toBeTrue();
  }

  <<DataProvider('provideCreate')>>
  public function testDowngradeByFirstName(
    Connection $conn,
    Migrator $migrator,
  ): void {
    \HH\Asio\join($migrator->upgrade());
    $result =
      \HH\Asio\join($migrator->downgradeTo('20150825102100-create-posts'));

    expect($result->resultCount())->toBeSame(1);

    $posts = $result->at('20150825102100-create-posts');
    expect($posts->containsKey(0))->toBeTrue();

    expect($result->containsKey('20150824010439-create-users'))->toBeFalse();
  }

  <<DataProvider('provideCreate')>>
  public function testDowngradeAll(Connection $conn, Migrator $migrator): void {
    \HH\Asio\join($migrator->upgrade());
    $result = \HH\Asio\join($migrator->downgrade());

    expect($result->resultCount())->toBeSame(2);

    $posts = $result->at('20150825102100-create-posts');
    expect($posts->containsKey(0))->toBeTrue();

    $users = $result->at('20150824010439-create-users');
    expect($users->containsKey(0))->toBeTrue();
  }

  <<DataProvider('provideCreate')>>
  public function testDowngradeMigrationNotFound(
    Connection $conn,
    Migrator $migrator,
  ): void {
    \HH\Asio\join($migrator->upgrade());

    expect(() ==> {
      \HH\Asio\join($migrator->downgradeTo('20150825102999-not-found'));
    })->toThrow(MigrationNotFoundException::class);
  }

}
