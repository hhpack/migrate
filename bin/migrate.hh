#!/usr/bin/env hhvm
<?hh //partial

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

$autoloadFiles = [
  __DIR__.'/../vendor/hh_autoload.php',
  __DIR__.'/../../../hh_autoload.php',
  __DIR__.'/../vendor/autoload.php',
  __DIR__.'/../../../autoload.php',
];

$composerInstall = false;
foreach ($autoloadFiles as $autoloadFile) {
  if (!file_exists($autoloadFile)) {
    continue;
  }
  require $autoloadFile;
  $composerInstall = true;
  break;
}
if (!$composerInstall) {
  echo
    'You must set up the dependencies, run the following commands:'.
    PHP_EOL.
    'curl -s http://getcomposer.org/installer | php'.
    PHP_EOL.
    'php composer.phar install'.
    PHP_EOL
  ;
  exit(1);
}
unset($composerInstall, $autoloadFiles, $autoloadFile);

(new Kernel())->run($argv);
