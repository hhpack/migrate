<?hh //strict

/**
 * This file is part of hhpack\migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace hhpack\migrate;

/**
 * name of migration
 * 
 * example:
 *   20150824010439-create-users
 *   20150825102100-create-posts
 */
type MigrationName = string;

/**
 * string value of data source name
 * 
 *   mysql:dbname=test;port=3306
 */
type DSNString = string;
