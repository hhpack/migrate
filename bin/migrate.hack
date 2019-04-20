/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate;

use HHPack\Migrate\Application\{Kernel};

function load_autoload_file(): void {
  $autoloadFiles = [
    __DIR__.'/../vendor/hh_autoload.hh',
    __DIR__.'/../../../hh_autoload.hh',
    __DIR__.'/../vendor/autoload.hh',
    __DIR__.'/../../../autoload.hh',
  ];

  $composerInstall = false;
  foreach ($autoloadFiles as $autoloadFile) {
    if (!\file_exists($autoloadFile)) {
      continue;
    }
    require $autoloadFile;
    $composerInstall = true;
    break;
  }
  if (!$composerInstall) {
    echo 'You must set up the dependencies, run the following commands:'.
      \PHP_EOL.
      'curl -s http://getcomposer.org/installer | php'.
      \PHP_EOL.
      'php composer.phar install'.
      \PHP_EOL;
    exit(1);
  }
}

<<__EntryPoint>>
function main(): noreturn {
  load_autoload_file();

  (new Kernel())->run(/* HH_IGNORE_ERROR[2050] */ $GLOBALS['argv']);

  exit(0);
}
